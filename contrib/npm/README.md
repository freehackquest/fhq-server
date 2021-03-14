


```
$ cd fhq-server
$ ./build_simple.sh
$ ./fhq-server export-libfhqcli-web-javascript
$ rsync -rtv ./freehackquest-libclient-web-js/ ../../freehackquest-libclient-web-js.git
$ npm login
Username: sea5kg
Password: *
Email: (this IS public) mrseakg@gmail.com
$ npm publish .
```