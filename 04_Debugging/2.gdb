b 28 if ((((i - start) / step + 1) > 27) && (((i - start) / step + 1) < 36))
commands 1
    printf "@@@%d %d %d %d\n", start, stop, step, i
    continue
end

run -100 100 3 > /dev/null
quit
