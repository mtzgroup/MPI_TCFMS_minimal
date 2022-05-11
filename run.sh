
mpiicpc server.cpp -o server
mpiicpc client.cpp -o client

server=tcfms_port$[ ( $RANDOM % 10000 ) + 1]
printf "&tc\nserver_name = $server" > tc_input

if [ -z $(ps aux | grep hydra_nameserver | grep -v "grep" | awk '{print $2}') ]; then hydra_nameserver & sleep 5; fi

mpiexec.hydra -nameserver localhost -n 1 ./server $server >& s.out &
sleep 2
mpiexec.hydra -nameserver localhost -n 1 ./client >& c.out &

