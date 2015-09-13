<?php
require(__DIR__.'/src/debug.hh');
$Daemon = new Debug('HG:DAEMON');
$Mailer = new Debug('HG:DAEMON:MAILER');

// $Daemon->enable();
// $Mailer->enable();

$Daemon->log('Staring Mailer');
$Mailer->log('Booting up');
$Mailer->log('Booted up');
$Daemon->log('Terminating Mailer');
$Mailer->log('Terminating');
