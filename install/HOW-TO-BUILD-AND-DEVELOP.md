cd ../fhq-server/fhq-web-user  
npm run build  
mkdir -p ../data_travis/public/games  
./fhq-server -wd ../data_travis/ -set "server_folder_public='pwd'/../data_travis/public/"
./fhq-server -wd ../data_travis start