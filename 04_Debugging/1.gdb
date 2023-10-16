b 28 if (i % 5 == 0)
commands 1
    printf "@@@%d %d %d %d\n", start, stop, step, i
    continue
end

run 1 12 > /dev/null
quit
