indexes = [28, 29, 30, 31, 32, 33, 34, 35]
el_break = gdb.Breakpoint("range_get", type=gdb.BP_BREAKPOINT, internal=True)
el_break.ignore_count = indexes[0] - 1
el_break.commands = 'printf "@@@ Start - %d, Stop - %d, Step - %d, Current - %d\\n", I->start, I->stop, I->step, I->current'
el_break.silent = True
gdb.execute('printf "Test2 started.\\n"')
try:
    gdb.execute("run -100 100 3 > /dev/null", to_string=True)
    for cur_index_id in range(1, len(indexes)):
        el_break.ignore_count = indexes[cur_index_id] - indexes[cur_index_id - 1] - 1
        gdb.execute("continue")
    el_break.delete()
    gdb.execute("continue") # finishing
except:
    gdb.execute('printf "@@@ Failed: not enough indexes.\\n"')
    pass
gdb.execute('printf "Test2 over.\\n"')
