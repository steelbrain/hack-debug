Hack-Debug
==========

Hack-Debug is a port of the awesome, famous node [debug][1] module to Hack/PHP.

#### Why use it?

Hack-Debug provides colored output for different modules which is nicer for the eyes and easier to debug. All the output from a single debug instance is of the same color. but when stdout is not a tty, it automatically appends an absolute date time to help the logging programs. It also automatically checks the `DEBUG` env variable to choose which debugger to enable/disable. For example if you do `DEBUG=MYAPP:* hhvm myApp.php` it'll enable these debuggers `MYAPP:DEBUGGER`, `MYAPP:MAILER`, `MYAPP:INSTALLER` but will not enable `MAILER:AUTH` .

#### API

```php
<?hh
class Debug {
  public function __construct(string $Prefix, ?resource $Output = STDOUT, bool $Enabled = Detect)
  public function enable(): void
  public function disable(): void
  public function log(string $Content): void
  public static function Log(string $Prefix, string $Content): void
}
```

#### Example Usage

```php
<?php
$Daemon = new Debug('HG:DAEMON');
$Mailer = new Debug('HG:DAEMON:MAILER');

// Enable debugger even when env DEBUG says otherwise
$Daemon->enable();
$Mailer->enable();

$Daemon->log('Staring Mailer');
$Mailer->log('Booting up');
$Mailer->log('Booted up');
$Daemon->log('Terminating Mailer');
$Mailer->log('Terminating');

```

#### LICENSE

This project is licensed under the terms of MIT License. See the license file for more info.

[1]:https://www.npmjs.com/package/debug
