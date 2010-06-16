#VIEW CODING ORDER 0-2-1
#echo '<SCRIPT> Encoding view 0...'
#./H264AVCEncoderLibTestStatic -vf ballroom_qp32.cfg 0
#echo '<SCRIPT> Encoding view 2...'
#./H264AVCEncoderLibTestStatic -vf ballroom_qp32.cfg 2
echo '<SCRIPT> Encoding view 1...'
./H264AVCEncoderLibTestStatic -vf ballroom_qp32.cfg 1

mousepad tracing.txt &