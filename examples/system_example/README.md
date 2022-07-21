
## System Example

was built a little example where is added the initial workflow of Mesh4All app

there are two modes Dodag and dag every mode has a workflow the working example is:

Dogag(master node) this node executes a server UDP and receives all data sent by node dag, one-time UDP server gets data,
this data is sent through uart to the esp32 the esp32 will receive the data and it will send the data to the server.

DAG (slave node) this node keeps connected to the sensors there is defined a time where get the sensor's information and serialize
the information with CBOR protocol and send it to the Dodag node through a UDP client

### Instructions to compile

To test this example you must have at least two boards samr21 and one board esp32(Wrover-kit or Wroom) and temperature sensor ds18

you must configure his nodes dodag or dag
connect the Dodag node to esp32 and the board you selected like Dodag through uart
connect the sensor ds18 to the dag node
cd examples/system_example

`make flash term`
