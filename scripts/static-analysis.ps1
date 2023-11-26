cd $PSScriptRoot
cd ..

$sources=Get-ChildItem ./ -recurse -Exclude ThirdParty | Select-Object -Expand FullName | where {$_ -Match "^((?!build)(?!ThirdParty)(?!tests).)*\.(cpp|h|hpp)$"}

echo $sources

Invoke-Expression "clang-tidy --config-file=.clang-tidy $sources"
exit $LASTEXITCODE
