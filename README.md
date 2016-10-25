# flexible logger for C.

This library attempts to produce a flexible, runtime
configurable, shared library for C applications.  The
idea is to create channels for output from expandable
classes and associate those objects to the lines of
code where the logs are called.  The logs are identified
by criticity, filename, line number, and function name,
and you create dynamically (in the software) which
associations to make to the output channels, so you
can filter logs with maximum granularity and by source
code reference.

As C has a flat namespace, we have selected the source
file name (available from `__FILE__`, `__LINE__` and
`__func__` macros) and create a database of filters to
apply to log messages that allow to process log entries
as you like.

Module is expandable by the use of drivers that allow
to specialize logs (so, for example to log to files,
irc channels, standar error, console, etc.)

Routines allow to define which members are to be
output in each channel, so uniform but flexible
logs can be produced (no interest for example in
including timestamp or file line in output to an irc
channel, but do it on file) and also rotating logs
is possible depending on driver used.
