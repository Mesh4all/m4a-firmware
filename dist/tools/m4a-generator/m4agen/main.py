"""m4agen main module."""

import os
import click

from .example import generate_example
from .board import generate_board
from .module import generate_module
from .test import generate_test
from . import __version__


class SharedCommand(click.core.Command):
    """Class for shared subcommand options"""

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        options = [
            click.core.Option(
                (
                    "-i",
                    "--interactive",
                ),
                is_flag=True,
                help="Use interactive mode",
            ),
            click.core.Option(
                (
                    "-c",
                    "--config",
                ),
                type=click.File(mode="r"),
                help="Use a configuration file",
            ),
            click.core.Option(
                ("-r", "--riotbase"),
                type=click.Path(exists=True),
                default=os.getenv("RIOTBASE"),
            ),
        ]
        self.params += options


@click.group()
@click.version_option(version=__version__)
def m4agen():  # pylint:disable=missing-function-docstring
    pass


@m4agen.command(cls=SharedCommand, help="Bootstrap a m4a-firmware board support")
def board(interactive, config, riotbase):
    """Entry point for board subcommand."""
    generate_board(interactive, config, riotbase)


@m4agen.command(cls=SharedCommand, help="Bootstrap a m4a-firmware example application")
def example(interactive, config, riotbase):
    """Entry point for example application subcommand."""
    generate_example(interactive, config, riotbase)


@m4agen.command(cls=SharedCommand, help="Bootstrap a m4a-firmware system module")
def module(interactive, config, riotbase):
    """Entry point for module subcommand."""
    generate_module(interactive, config, riotbase)


@m4agen.command(cls=SharedCommand, help="Bootstrap a m4a-firmware test application")
def test(interactive, config, riotbase):
    """Entry point for test subcommand."""
    generate_test(interactive, config, riotbase)
