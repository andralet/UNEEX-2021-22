el_break = gdb.Breakpoint("range_get", type=gdb.BP_BREAKPOINT, internal=True)
el_break.condition = "I->current % 5 == 0"
el_break.commands = 'printf "@@@ Start - %d, Stop - %d, Step - %d, Current - %d\\n", I->start, I->stop, I->step, I->current'
el_break.silent = True
gdb.execute('printf "Test1 started.\\n"')
try:
    gdb.execute("run 1 12 > /dev/null", to_string=True)
    while True:
        gdb.execute("continue")
except:
    pass
gdb.execute('printf "Test1 over.\\n"')
