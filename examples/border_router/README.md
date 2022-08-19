Hosts Intercommunication example (Border router)
=============

Stating with the first border_router:
```
make -C examples/border_router flash install HOST_IPV6=2001:db8::1
```
The Host communication with the network interface is implemented the chamoc_tool
in `dist/tools/chamoc`. This starts sending an nib request to get an ipv6 route to
the USB interface. This will works in both border routers

```
make -C examples/border_router flash install HOST_IPV6=2001:db9::1
```

When you are running this example, the network prefix length will be {HOST_IPV6}/128.
This means that only identify an only address in the wired interface.

well you need to add the default rote to redirect the unknown network address to the border_router 2.
```
nib add 6 ::/0 2001:db8:2:0:204:2519:1801:cad4
```
or could be added via link local address of border_router 2.
 ```
nib add 6 ::/0  fe80::204:2519:1801:cad4 dev
```