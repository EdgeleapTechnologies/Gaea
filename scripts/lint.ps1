cd $PSScriptRoot
cd ..
python3 ./scripts/run-clang-format.py --style file -e **/ThirdParty/** -e **/build/** -r .
exit $LASTEXITCODE
