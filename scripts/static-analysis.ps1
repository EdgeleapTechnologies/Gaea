cd $PSScriptRoot
cd ..

$common_sources=get-ChildItem ./ -recurse | Select-Object -Expand FullName | where {$_ -like "*.cpp"}
$common_sources+=" "
$common_sources+=get-ChildItem ./ -recurse | Select-Object -Expand FullName | where {$_ -like "*.h"}
$common_sources+=" "
$common_sources+=get-ChildItem ./ -recurse | Select-Object -Expand FullName | where {$_ -like "*.hpp"}

Invoke-Expression "clang-tidy --config-file=.clang-tidy $common_sources"
exit $LASTEXITCODE
