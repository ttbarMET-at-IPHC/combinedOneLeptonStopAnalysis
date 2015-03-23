



make clean compile

mkdir outputs

./mapProducer_BDT.run nominal T2tt
./mapProducer_BDT.run nominal T2bw-025
./mapProducer_BDT.run nominal T2bw-050
./mapProducer_BDT.run nominal T2bw-075
mv outputs SCC_nominal && mkdir outputs

./mapProducer_BDT.run T2tt_left T2tt
mv outputs SCC_T2tt_left && mkdir outputs

./mapProducer_BDT.run T2tt_right T2tt
mv outputs SCC_T2tt_right && mkdir outputs

./mapProducer_BDT.run T2bw_ll T2bw-025
./mapProducer_BDT.run T2bw_ll T2bw-050
./mapProducer_BDT.run T2bw_ll T2bw-075
mv outputs SCC_T2bw_ll && mkdir outputs

./mapProducer_BDT.run T2bw_rl T2bw-025
./mapProducer_BDT.run T2bw_rl T2bw-050
./mapProducer_BDT.run T2bw_rl T2bw-075
mv outputs SCC_T2bw_rl && mkdir outputs

./mapProducer_BDT.run T2bw_lr T2bw-025
./mapProducer_BDT.run T2bw_lr T2bw-050
./mapProducer_BDT.run T2bw_lr T2bw-075
mv outputs SCC_T2bw_lr && mkdir outputs

./mapProducer_BDT.run T2bw_rr T2bw-025
./mapProducer_BDT.run T2bw_rr T2bw-050
./mapProducer_BDT.run T2bw_rr T2bw-075
mv outputs SCC_T2bw_rr && mkdir outputs

