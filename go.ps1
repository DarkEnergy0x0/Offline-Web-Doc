param(
    [String]$vpRootPath=$PSScriptRoot + '\vuepress\',
    [String]$vpProjName='doc',
    # Parameter help description
    [Parameter(Position=0)]
    [String]$upxPath=$PSScriptRoot + '\upx.exe'
)

$vpProjPath = "${vpRootPath}\${vpProjName}"
$genHPath = "${PSScriptRoot}\genH.exe"
$paths = $vpRootPath, $upxPath, $genHPath

# Checks
## Check the paths
foreach($path in $paths) {
    if (-not (Test-Path $path)) {
        Write-Host "Invalid path: ${path}"
        Exit
    }
}

# Run jobs
## Build static files by vuepress
if (-not (Test-Path "${vpProjPath}" -PathType Container)) {
    Write-Host "vpProjPath not found: ${vpProjPath}"
    Exit
}

if (Test-Path "${PSScriptRoot}\dist" -PathType Container) {
    Write-Host "Deleting old dist folder.."
    Remove-Item -Force "${PSScriptRoot}\dist\" 
}

Write-Host "Building static files..."
Set-Location $vpRootPath
npx vuepress build ${vpProjName}
Set-Location $PSScriptRoot
## Check the output dir for vuepress
if (-not (Test-Path "${vpProjPath}\.vuepress\dist" -PathType Container)) {
    Write-Host "Not found output dir for vuepress"
    Exit
}

Write-Host "Moving new dist folder to the root dir for project..."
Move-Item "${vpProjPath}\.vuepress\dist" $PSScriptRoot

# Generating header file
Write-Host "Generating header file..."
& $genHPath

# check build folder
if (-not (Test-Path "${PSScriptRoot}\build" -PathType Container)) {
    Write-Host "Create folder for build"
    mkdir "build"
} else {
    if (Test-Path "main.exe" -PathType Leaf) {
        Write-Host "Remove old executable file"
        Remove-Item "main.exe"
    }
}

Set-Location 'build'
Write-Host "Building finally executable file..."
cmake -DCMAKE_BUILD_TYPE=MINSIZEREL ..


# This is command used to build, what command to use depends on your build tools, you can modify it according your case
mingw32-make.exe

if (Test-Path 'main.exe' -PathType Leaf) {
    Move-Item -Force "main.exe" $PSScriptRoot
    Write-Host "The executable file generated at: ${PSScriptRoot}\main.exe"
} else {
    Write-Host "Not found the executable file for generated"
}
Set-Location $PSScriptRoot

Write-Host "Compressing the executable file using UPX..."
./upx.exe --best main.exe