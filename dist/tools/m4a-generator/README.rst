m4agen: a RIOT source code generator
-------------------------------------

|CI| |codecov| |PyPi|

.. |CI| image:: https://github.com/aabadie/riot-generator/workflows/CI/badge.svg
    :target: https://github.com/aabadie/riot-generator/actions?query=workflow%3ACI+branch%3Amaster
    :alt: CI status

.. |codecov| image:: https://codecov.io/gh/aabadie/riot-generator/branch/master/graph/badge.svg
  :target: https://codecov.io/gh/aabadie/riot-generator

.. |PyPi| image:: https://badge.fury.io/py/m4agen.svg
    :target: https://badge.fury.io/py/m4agen
    :alt: m4agen version

``m4agen`` is a command line interface helper tool that is used to bootstrap
`RIOT <http://github.com/RIOT-OS/RIOT>`_ source files for standalone applications,
board supports, driver modules, system modules, packages and example/test applications.

``m4agen`` is **Python 3.7+** compatible.

Installation
............

Install ``m4agen`` using ``pip``::


Install using ``pip`` from the source directory::

    cd dist/tools/m4a-generator
    pip install .

Usage
.....

``m4agen`` uses subcommands for generating the code for applications,
tests, packages and board support::

    m4agen --help
    Usage: m4agen [OPTIONS] COMMAND [ARGS]...

    Options:
      --version  Show the version and exit.
      --help     Show this message and exit.

    Commands:
      application  Bootstrap a RIOT application
      board        Bootstrap a RIOT board support
      driver       Bootstrap a RIOT driver module
      example      Bootstrap a RIOT example application
      module       Bootstrap a RIOT system module
      pkg          Bootstrap a RIOT external package
      test         Bootstrap a RIOT test application


Examples
........

Generate an application in the current directory that build against the RIOT
source located in /opt/RIOT and using the interactive wizzard::

    m4agen application -i -r /opt/RIOT

or::

    RIOTBASE=/opt/RIOT m4agen application -i

The command line wizard will ask for questions about the new
application: target board, RIOT base directory, author name, etc.

Generate an application using a configuration file (see the
`samples <https://github.com/aabadie/riot-generator/tree/master/m4agen/samples>`_
provided in the source code)::

    m4agen application --riotbase /opt/RIOT --config path/to/config/file.cfg


In both cases, once complete, the new application can be built using::

    make

``example``, ``driver``, ``module``, ``pkg``, ``test`` and ``board`` subcommands generate
the skeleton code directly in the RIOT base directory::

    m4agen example --riotbase /opt/RIOT -i
    m4agen driver --riotbase /opt/RIOT -i
    m4agen module --riotbase /opt/RIOT -i
    m4agen pkg --riotbase /opt/RIOT -i
    m4agen board --riotbase /opt/RIOT -i
    m4agen test --riotbase /opt/RIOT -i


Testing
.......

Testing is performed using the `Tox <http://github.com/tox-dev/tox>`_
automation tool. You can install Tox using pip::

    pip install tox

To run the whole tests and checks, use::

    tox

To only run the tests, use::

    tox -e tests

To only run the static checks (``flake8``, ``black``, ``twine --check``), use::

    tox -e check

To reformat your code following the `black <https://black.readthedocs.io/en/stable>`_
tool, use::

    tox -e format
