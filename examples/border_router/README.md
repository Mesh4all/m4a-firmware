Hosts Intercommunication example (Border router)
=============

Stating with the first border_router:
```
make -C examples/border_router menuconfig all
```
in the kconfig parameters assign the rpl settings as a `DODAG` node, then assign the ipv6 address in both interfaces, Wired is `2001:db8::2` and the Wireless is `2001:db8:2::1`
```
then
```
make -C examples/border_router flash term HOST_IPV6=2001:db8::1
```
to second border router
```
make -C examples/border_router menuconfig
```
then
```
make -C examples/border_router flash term HOST_IPV6=2001:db8::1
```
in the kconfig parameters assign the rpl settings as a `DAG` node, then assign the ipv6 address in only wired interfaces, Address  is `2001:db9::2

when both border routers are working we need to set the nib route general in border_router to the `2001:db9::/64` network.
```
> nib route
2022-08-02 01:19:56,196 # 2001:db8:2::/64 dev #6
2022-08-02 01:19:56,198 # 2001:db8::/64 dev #7
2022-08-02 01:19:56,205 # 2001:db8:2:0:204:2519:1801:cad4/128 via fe80::204:2519:1801:cad4 dev #6
```
well you need to add the default rote to redirect the unknown network address to the border_router 2.
```
nib add 6 ::/0 2001:db8:2:0:204:2519:1801:cad4
```
or could be added via link local address of border_router 2
 ```
nib add 6 ::/0  fe80::204:2519:1801:cad4 dev
```