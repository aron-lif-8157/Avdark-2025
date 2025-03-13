echo "removing old performance runs"
rm -f performance.txt

echo "lock test" >> performance.txt

for i in {1..4}; do
  echo "running lock test performance check $i"
  output=$(./test_atomic_counter lock)
# Extract the performance line
  perf_line=$(echo "$output" | grep "Performance:")
# append the performance line to a file
  echo "$perf_line" >> performance.txt
done

echo "inc/dec atomic test" >> performance.txt

for i in {1..4}; do
  echo "running inc/dec atomic performance check $i"
  output=$(./test_atomic_counter atomic_incdec)
# Extract the performance line
  perf_line=$(echo "$output" | grep "Performance:")
# append the performance line to a file
  echo "$perf_line" >> performance.txt
done

echo "compare exchange atomic test" >> performance.txt
for i in {1..4}; do
  echo "running compare exchange atomic performance check $i"
  output=$(./test_atomic_counter atomic_cas)
# Extract the performance line
  perf_line=$(echo "$output" | grep "Performance:")
# append the performance line to a file
  echo "$perf_line" >> performance.txt

done



echo "Performance data saved to performance.txt"