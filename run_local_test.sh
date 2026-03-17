docker build -t cpp-runner . --quiet

echo "\nTime Usage:\n"

time docker run --rm -it -v "$(pwd)/data/input:/input" -v "$(pwd)/data/output:/output" cpp-runner "$1" "$2"

echo "\nOutput:\n"

cat "data/$2"

echo "\n"