+10 [ # Initialize cell 0 to 10
    > +7   # cell 1 = 7
    > +10  # cell 2 = 10
    > +3   # cell 3 = 3
    > +    # cell 4 = 1
    <4 -   # move pointer left 4 and decrement cell 0
] 
> +2 .    # cell 1 + 2, print 'H' (72)
> +  .    # cell 2 + 1, print 'e' (101)
+7 . .    # cell 2 +7, print 'l' 'l' 'o' (108 x3)
+3 .
> +2 .    # cell 3 +2, print ' ' (32)
<2 +15 .  # move pointer left 2, add 15, print 'W' (87)
> .+3 .      # cell 3 print 'o' (111)
-6 .     # cell 3 -6 print 'l' (108)
-8 .     # cell 3 -8 print 'd' (100)
> + .   # cell 4 +1 print '!' (33)
> .     # cell 5 print '\n' (10)
