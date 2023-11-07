cd "$(dirname "$0")"
cd ..

python3 ./scripts/run-clang-format.py --style file -r ./ --exclude ThirdParty --exclude build
