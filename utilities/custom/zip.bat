
set src=%3
set dest=%2
powershell -c 'Compress-Archive -Path %src%  -CompressionLevel Optimal -DestinationPath %dest%'