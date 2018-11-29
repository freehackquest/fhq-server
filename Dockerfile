FROM debian:9.5

EXPOSE 1234 4613
COPY ./fhq-server/fhq-server.debian95 /usr/bin/fhq-server
RUN mkdir -p /usr/share/fhq-server/public/games 
RUN mkdir -p /var/log/fhq-server

RUN apt-get update && apt-get install -y libcurl3 zlibc zlib1g libpng16-16 libmariadbclient18

RUN apt-get install -y libqt5sql5-mysql
RUN apt-get install -y libqt5websockets5

# RUN apt-get install -y qt5-default qtchooser \

RUN apt-get clean

CMD fhq-server start