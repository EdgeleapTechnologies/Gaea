cd $PSScriptRoot
cd ..

$compiler_name="clang"
$compiler="Clang"
$generator="Ninja"
$build_test=$false
$test_only=$false
$build_type="Debug"
$build_type_name="debug"
if($args.count -gt 0)
{
    for ( $i = 0; $i -lt $args.count; $i++ ) {
        switch($args[$i])
        {
            "--clang" {
                $generator="Ninja"
                $compiler_name="clang"
                $compiler="Clang"
            }
            "--msvc" {
                $generator="Visual Studio 17 2022"
                $compiler_name="msvc"
                $compiler="cl.exe"
            }
            "--test" {
                $build_test=$true
            }
            "--test-only" {
                $build_test=$true
                $test_only=$true
            }
            "--release" {
                $build_type="Release"
                $build_type_name="release"
            }
            "--debug" {
                $build_type="Debug"
                $build_type_name="debug"
            }
        }
    } 
}

#setup the build folder and run cmake generator
if (-not (Test-Path -LiteralPath ./build/))
{
    mkdir build > $null
}
if (-not (Test-Path -LiteralPath ./build/windows-$compiler_name-$build_type_name)) {
    mkdir build/windows-$compiler_name-$build_type_name > $null
    cd build/windows-$compiler_name-$build_type_name
    cmake -G $generator -DCMAKE_C_COMPILER="$compiler" -DCMAKE_CXX_COMPILER="$compiler" ../.. -DCMAKE_BUILD_TYPE="$build_type"
    cd ../..
}

#build the targets
cd ./build/windows-$compiler_name-$build_type_name
if($test_only -eq $false)
{
    cmake --build . -j --config $build_type
}
if($build_test -eq $true)
{
    cmake --build . -j --target gaea-tests --config $build_type
}
$outcode = $LASTEXITCODE

#post-build executions
if (Test-Path -LiteralPath compile_commands.json) {
    cp compile_commands.json ../..
}
cd ../..

exit $outcode
