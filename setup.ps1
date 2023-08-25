# Copying the dlls
$sourcePath = ".\Libraries\lib\"
$destinationPath = "\build\VS2022\bin\x64\Debug"

if (-Not (Test-Path -Path $destinationPath)) {
    New-Item -ItemType Directory -Force -Path $destinationPath | Out-Null
}

$dllFiles = Get-ChildItem -Path $sourcePath -Filter "*.dll" -File

foreach ($dllFile in $dllFiles) {
    $destinationFile = Join-Path -Path $destinationPath -ChildPath $dllFile.Name
    Copy-Item -Path $dllFile.FullName -Destination $destinationFile -Force
}

Write-Host "DLLs copied to $destinationPath" -ForegroundColor Green


# Checking for the whisper model
# $filePath = "..\Models\ggml-model-whisper-base.en.bin"

# $downloadURL = "https://ggml.ggerganov.com/ggml-model-whisper-base.en.bin"
# if (-Not(Test-Path -Path $filePath -PathType Leaf)) {
#     Write-Host "ERROR: The file '$filePath' does not exist." -ForegroundColor Red
#     Write-Host "Please download the file from the following URL:"
#     Write-Host $downloadURL
#     Write-Host "And place it under the path: $filePath"
#     return;
# }
# Write-Host "Whisper model file found!" -ForegroundColor Green

# MSBuild.exe .\Echo.sln /t:Build /p:Configuration=Debug

Write-Host "Setup complete!" -ForegroundColor Green


