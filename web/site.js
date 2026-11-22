/* Landing-page helpers only. Terminal wiring stays in app.js. */
(function () {
  "use strict";

  /* xterm.js can open at full size but still drop the first paint when the
     terminal starts below the fold. If the menu never appears, Restart once
     the terminal is in view. Visible first-loads are left alone. */
  var term = document.getElementById("terminal");
  var restart = document.getElementById("restart");
  var kicked = false;

  function menuVisible() {
    var rows = term && term.querySelector(".xterm-rows");
    var text = rows ? (rows.innerText || "") : "";
    return /Enter choice|RUPEECLARITY|RupeeClarity/i.test(text);
  }

  function kick() {
    if (kicked || !restart) return;
    if (menuVisible()) {
      kicked = true;
      return;
    }
    kicked = true;
    window.dispatchEvent(new Event("resize"));
    restart.click();
  }

  if (term && restart) {
    if ("IntersectionObserver" in window) {
      var io = new IntersectionObserver(function (entries) {
        if (entries.some(function (e) { return e.isIntersecting; })) {
          io.disconnect();
          kick();
        }
      }, { threshold: 0.01 });
      io.observe(term);
    } else {
      window.addEventListener("load", kick);
    }
  }

  var btn = document.getElementById("copy-install");
  var cmd = document.getElementById("install-cmd");
  var live = document.getElementById("copy-status");
  if (!btn || !cmd) return;

  function announce(message) {
    if (live) live.textContent = message;
  }

  function copied() {
    var original = btn.textContent;
    btn.textContent = "Copied";
    announce("Install command copied to clipboard.");
    window.setTimeout(function () {
      btn.textContent = original;
      announce("");
    }, 2000);
  }

  function failed() {
    announce("Could not copy. Please select the command and copy it yourself.");
  }

  btn.addEventListener("click", function () {
    var text = cmd.textContent.replace(/^\s+|\s+$/g, "");
    if (navigator.clipboard && navigator.clipboard.writeText) {
      navigator.clipboard.writeText(text).then(copied, failed);
      return;
    }
    var range = document.createRange();
    range.selectNodeContents(cmd);
    var sel = window.getSelection();
    sel.removeAllRanges();
    sel.addRange(range);
    try {
      if (document.execCommand("copy")) copied();
      else failed();
    } catch (e) {
      failed();
    }
    sel.removeAllRanges();
  });
})();
