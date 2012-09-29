
in=../sw1000.txt
for i in 5 10 20 30 40 50 ;do
    ./LLEeigen $in 1000 $i > result1000_${i}.txt
done