echo "removing old performance runs"
rm -f timing_B.txt

echo "lock test" >> timing_B.txt

for i in {1..4}; do
  echo "running mutex test performance check $i"
  output=$(./test_user_lock mutex)
# Extract the performance line
  perf_line=$(echo "$output" | grep "Performance:")
# append the performance line to a file
  echo "$perf_line" >> timing_B.txt
done

echo "inc/dec atomic test" >> timing_B.txt

for i in {1..4}; do
  echo "running inc/dec atomic performance check $i"
  output=$(./test_user_lock dekker)
# Extract the performance line
  perf_line=$(echo "$output" | grep "Performance:")
# append the performance line to a file
  echo "$perf_line" >> timing_B.txt
done




echo "Performance data saved to timing_B.txt"