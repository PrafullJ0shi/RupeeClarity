#!/usr/bin/env python3
"""Scripted-stdin tests for every RupeeClarity module.
Usage: python3 tests/test_cli.py ./rupeeclarity   (run from any folder; module 9 writes a report in the current folder)
"""
import subprocess, sys, os
BIN=sys.argv[1]
def run(inp):
    r=subprocess.run([BIN],input=inp.encode(),capture_output=True,timeout=10,env={**os.environ,"TERM":"dumb"})
    return r.returncode, r.stdout.decode('utf-8','replace')
tests=[
 ("1 SI",            "1\n10000\n10\n2\n\n0\n", ["Simple Interest : Rs 2000.00","Total Amount    : Rs 12000.00"]),
 ("2 CI yearly",     "2\n10000\n10\n2\n1\n\n0\n", ["Compound Interest : Rs 2100.00","Total Amount      : Rs 12100.00"]),
 ("3 Compare",       "3\n10000\n10\n2\n1\n\n0\n", ["Simple Interest     : Rs 2000.00","Compound Interest   : Rs 2100.00","Extra (CI - SI)     : Rs 100.00","Winner              : Compound Interest"]),
 ("4 Year table",    "4\n10000\n10\n2\n1\n\n0\n", ["   1 | Rs   11000.00 | Rs   11000.00 | Rs 0.00","   2 | Rs   12000.00 | Rs   12100.00 | Rs 100.00"]),
 ("5 Goal",          "5\n10000\n10\n2\n1\n12050\n\n0\n", ["SI Amount: Rs 12000.00  --> NOT REACHED","CI Amount: Rs 12100.00  --> REACHED"]),
 ("6 What-if",       "6\n10000\n10\n2\n1\n\n0\n", ["CI Amount at 11.00% : Rs 12321.00","Difference          : Rs 221.00"]),
 ("7 Rule72",        "7\n8\n\n0\n", ["9.00 years"]),
 ("8 Story",         "8\n10000\n10\n2\n1\n\n0\n", ["(Rs 1000.00)","Rs 100.00 extra"]),
 ("9 Report",        "9\n10000\n10\n2\n1\n\n0\n", ["Report saved successfully"]),
 ("10 Formula",      "10\n\n0\n", ["RULE OF 72","Years to double ≈ 72 / R"]),
 ("11 About",        "11\n\n0\n", ["Class Roll No: 43","Ms. Shivani Sisodia"]),
 ("0 Exit",          "0\n", ["Thank you for using RupeeClarity."]),
 ("CI monthly",      "2\n10000\n10\n2\n4\n\n0\n", ["Compound Interest : Rs 2203.91"]),
 ("single ENTER back to menu", "10\n\n0\n", ["Thank you for using RupeeClarity."]),
]
inv=[
 ("menu 12abc/2.5/empty/12/-1/abc then 0", "12abc\n2.5\n\n12\n-1\nabc\n 0 \n", "Invalid choice. Please try again.", 6),
 ("P 12abc/inf/nan/-5/0/empty/abc/1e999/0x10 then ok", "1\n12abc\ninf\nnan\n-5\n0\n\nabc\n1e999\n0x10\n10000\n10\n2\n\n0\n", "Please enter a positive number.", 9),
 ("freq 0/5/1.5/2x", "2\n10000\n10\n2\n0\n5\n1.5\n2x\n1\n\n0\n", "Invalid choice. Please try again.", 4),
 ("long line", "1"*300+"\n0\n", "Invalid choice. Please try again.", 1),
]
ok=True
for name,inp,exp in tests:
    rc,out=run(inp); miss=[e for e in exp if e not in out]
    good = rc==0 and not miss and "Thank you" in out
    ok&=good; print(("PASS" if good else "FAIL"),name, miss)
for name,inp,msg,cnt in inv:
    rc,out=run(inp); c=out.count(msg); good = rc==0 and c==cnt and "Thank you" in out
    ok&=good; print(("PASS" if good else "FAIL"),name,f"{c}x '{msg}' (expected {cnt})")
for name,inp in [("EOF at menu",""),("EOF mid-input","1\n100\n"),("EOF at pause","7\n8\n")]:
    rc,out=run(inp); good= rc==0 and "Input closed (end of file). Exiting RupeeClarity." in out
    ok&=good; print(("PASS" if good else "FAIL"),name)
# single enter: after pause one empty line must lead to menu, not consume next
rc,out=run("7\n8\n\n7\n9\n\n0\n"); good="8.00 years" in out and out.count("Press ENTER")==2 and "Thank you" in out
ok&=good; print(("PASS" if good else "FAIL"),"one ENTER then next module")
print("ALL PASS" if ok else "SOME FAILED")
sys.exit(0 if ok else 1)
