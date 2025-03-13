echo "removing old performance runs"
rm -f timing_A.txt

echo "lock test" >> timing_A.txt

for i in {1..4}; do
  echo "running lock test performance check $i"
  output=$(./test_atomic_counter lock)
# Extract the performance line
  perf_line=$(echo "$output" | grep "Performance:")
# append the performance line to a file
  echo "$perf_line" >> timing_A.txt
done

echo "inc/dec atomic test" >> timing_A.txt

for i in {1..4}; do
  echo "running inc/dec atomic performance check $i"
  output=$(./test_atomic_counter atomic_incdec)
# Extract the performance line
  perf_line=$(echo "$output" | grep "Performance:")
# append the performance line to a file
  echo "$perf_line" >> timing_A.txt
done

echo "compare exchange atomic test" >> timing_A.txt
for i in {1..4}; do
  echo "running compare exchange atomic performance check $i"
  output=$(./test_atomic_counter atomic_cas)
# Extract the performance line
  perf_line=$(echo "$output" | grep "Performance:")
# append the performance line to a file
  echo "$perf_line" >> timing_A.txt

done



echo "Performance data saved to timing_A.txt"