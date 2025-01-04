FROM gcc:latest

RUN set -ex; \
    apt-get update; \
    apt-get install -y cmake libzmq3-dev;

WORKDIR /usr/src/app

COPY . .

RUN mkdir -p build && \ 
    cd build && \ 
    cmake .. && \
    make

EXPOSE 5566

CMD ["./build/smoke_test"]


