param (
    [string]$file
)

(Get-Content $file | Select-Object -Skip 7) | Set-Content $file