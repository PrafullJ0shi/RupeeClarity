/* RupeeClarity web terminal: connects the WebAssembly build of src/rupeeclarity.c
   to an xterm.js terminal. Input is collected line by line and handed to the
   program through Module.readLine (see web_read_line in the C source). */
(function () {
  "use strict";

  var COLS_NEEDED = 68;                 /* widest line the program prints is about 66 characters */
  var termEl = document.getElementById("terminal");
  var statusEl = document.getElementById("status");
  var lineEl = document.getElementById("line");
  var downloadBtn = document.getElementById("download");
  var isTouch = window.matchMedia("(pointer: coarse)").matches;

  var term = new Terminal({
    convertEol: true,
    cursorBlink: true,
    fontFamily: 'ui-monospace, SFMono-Regular, Menlo, Consolas, "DejaVu Sans Mono", "Liberation Mono", monospace',
    fontSize: 15,
    scrollback: 5000,
    theme: {
      background: "#0d1117", foreground: "#e6edf3", cursor: "#34d399",
      selectionBackground: "#1f6f5088", green: "#34d399", brightGreen: "#6ee7b7"
    }
  });
  var fit = new FitAddon.FitAddon();
  term.loadAddon(fit);
  term.open(termEl);

  function resize() {
    /* pick a font size so the 60-column menu fits, even on a phone */
    var width = termEl.clientWidth - 16;
    var size = Math.floor(width / (COLS_NEEDED * 0.6));
    size = Math.max(8, Math.min(15, size));
    if (term.options.fontSize !== size) term.options.fontSize = size;
    try { fit.fit(); } catch (e) { /* not visible yet */ }
  }
  window.addEventListener("resize", resize);
  if (window.visualViewport) window.visualViewport.addEventListener("resize", resize);
  resize();

  /* ---------- output: bytes from the C program -> UTF-8 text -> terminal ---------- */
  var outBytes = [];
  var decoder = new TextDecoder("utf-8");
  var flushTimer = null;
  function flushOut() {
    flushTimer = null;
    if (!outBytes.length) return;
    var chunk = new Uint8Array(outBytes);
    outBytes = [];
    term.write(decoder.decode(chunk, { stream: true }));
  }
  function putByte(b) {
    if (b === null || b === undefined) return;
    outBytes.push(b & 0xff);
    if (!flushTimer) flushTimer = setTimeout(flushOut, 0);
  }

  /* ---------- input: a simple line editor + the on-screen input box ---------- */
  var session = null;          /* { pending: resolve fn | null, queue: [], ended: bool, mod } */
  var typed = "";

  function deliver(text, echo) {
    if (!session || session.ended) return;
    if (echo) term.write(text + "\r\n");
    if (session.pending) {
      var resolve = session.pending;
      session.pending = null;
      resolve(text);
    } else {
      session.queue.push(text);
    }
  }

  term.onData(function (data) {
    if (!session || session.ended) {
      if (data === "\r") startSession();
      return;
    }
    if (data.charCodeAt(0) === 27) return;      /* ignore arrow keys and other escape sequences */
    for (var i = 0; i < data.length; i++) {
      var ch = data[i];
      if (ch === "\r" || ch === "\n") {
        var line = typed; typed = "";
        term.write("\r\n");
        deliver(line, false);
      } else if (ch === "\x7f" || ch === "\b") {
        if (typed.length) { typed = typed.slice(0, -1); term.write("\b \b"); }
      } else if (ch === "\x03") {               /* Ctrl+C: clear the current line */
        term.write("^C\r\n"); typed = "";
      } else if (ch === "\x04") {               /* Ctrl+D: end of input, like a real terminal */
        if (!typed) { session.eofRequested = true; deliver(null, false); }
      } else if (ch >= " ") {
        typed += ch; term.write(ch);
      }
    }
  });

  document.getElementById("inputbar").addEventListener("submit", function (ev) {
    ev.preventDefault();
    sendFromUi(lineEl.value);
    lineEl.value = "";
    if (isTouch) lineEl.focus();
  });
  Array.prototype.forEach.call(document.querySelectorAll("[data-send]"), function (btn) {
    btn.addEventListener("click", function () { sendFromUi(btn.getAttribute("data-send")); });
  });
  function sendFromUi(text) {
    if (!session || session.ended) { startSession(); return; }
    if (typed) { term.write("\b \b".repeat(typed.length)); typed = ""; }
    deliver(text, true);
    if (!isTouch) term.focus();
  }

  /* ---------- report file (menu option 9) ---------- */
  function reportBytes() {
    try { return session && session.mod && session.mod.FS.readFile("rupeeclarity_report.txt"); }
    catch (e) { return null; }
  }
  function refreshDownload() { downloadBtn.disabled = !reportBytes(); }
  downloadBtn.addEventListener("click", function () {
    var data = reportBytes();
    if (!data) return;
    var url = URL.createObjectURL(new Blob([data], { type: "text/plain" }));
    var a = document.createElement("a");
    a.href = url; a.download = "rupeeclarity_report.txt";
    document.body.appendChild(a); a.click(); a.remove();
    setTimeout(function () { URL.revokeObjectURL(url); }, 1000);
  });

  /* ---------- program lifecycle ---------- */
  function setStatus(text, cls) { statusEl.textContent = text; statusEl.className = "status " + (cls || ""); }

  function ended() {
    if (!session || session.ended) return;
    flushOut();
    session.ended = true;
    refreshDownload();
    setStatus("Ended", "ended");
    term.write("\r\n\x1b[33m[Program finished. Press Enter or tap Restart to run it again.]\x1b[0m\r\n");
  }

  function startSession() {
    var s = { pending: null, queue: [], ended: false, mod: null };
    if (session) session.ended = true;   /* the old run is left waiting and is never resumed */
    session = s;
    typed = "";
    outBytes = [];
    term.reset();
    downloadBtn.disabled = true;
    setStatus("Loading…");

    createRupeeClarity({
      readLine: function () {
        flushOut();
        if (s !== session) return new Promise(function () {});   /* old session: never resumes */
        refreshDownload();
        if (s.queue.length) return Promise.resolve(s.queue.shift());
        return new Promise(function (resolve) { s.pending = resolve; });
      },
      preRun: [function (mod) {
        mod.FS.init(function () { return null; }, putByte, putByte);
      }],
      onExit: function () { if (s === session) ended(); },
      onAbort: function () { if (s === session) ended(); },
      print: function () {}, printErr: function () {}
    }).then(function (mod) {
      s.mod = mod;
      if (s === session && !s.ended) setStatus("Running", "live");
    }).catch(function (err) {
      setStatus("Error", "ended");
      term.write("\r\nCould not start the program: " + err + "\r\n");
    });
    if (!isTouch) term.focus();
  }

  document.getElementById("restart").addEventListener("click", startSession);
  startSession();
})();
