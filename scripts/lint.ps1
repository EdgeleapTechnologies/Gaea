cd $PSScriptRoot
cd ..
python ./scripts/run-clang-format.py --style file -r ./ --exclude ThirdParty --exclude build
exit $LASTEXITCODE
