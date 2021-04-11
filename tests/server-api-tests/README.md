# server-api-tests

[docs by pytest](https://docs.pytest.org/)

Requiremets for python3:

```
$ pip3 install -r requirements.txt
```

## when server run in docker

Required:

* pyhton3
* docker

```
$ docker pull freehackquest/fhq-server
$ docker pull mysql:5.7
```

```
$ pytest-3 -rAs -c env-docker.ini tests_leaks.py
```

## when server locally builded

Update python client library:

```
$ ./update_freehackquest_libclient_py.sh
```

Run pylint:

```
$ pylint **/*.py
```

Run pytests:

```
$ pytest-3 -rAs -c env-travis.ini some.py
```


