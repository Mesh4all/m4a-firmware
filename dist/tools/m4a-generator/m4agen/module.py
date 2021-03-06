"""Mesh4all application generator module."""

import os
import click

from .common import load_and_check_params, check_overwrite, render_source


MODULE_PARAMS = {
    "name": {"args": ["Module name"], "kwargs": {}},
    "displayed_name": {
        "args": ["Module Doxygen name"],
        "kwargs": {},
    },
    "brief": {"args": ["Brief doxygen description"], "kwargs": {}},
}

MODULE_FILES = {
    "module.c": "{name}.c",
    "Makefile.include": None,
    "Makefile.dep": None,
    "Makefile": None,
}

MODULE_INCLUDE_FILES = {"module.h": "{name}.h"}


def generate_module(interactive, config, riotbase):
    """Generate the code of a module."""
    group = "module"
    params = load_and_check_params(
        group, MODULE_PARAMS, [], interactive, config, riotbase, "firmware"
    )

    sys_dir = os.path.join(riotbase, "firmware")
    output_dir = os.path.join(sys_dir, params[group]["name"])
    output_include_dir = os.path.join(output_dir, "include")
    check_overwrite(output_dir)
    render_source(params, group, MODULE_FILES, output_dir)
    render_source(params, group, MODULE_INCLUDE_FILES, output_include_dir)

    click.echo(
        click.style(
            f"Module '{params[group]['name']}' generated in {output_dir} with success!",
            bold=True,
        )
    )
