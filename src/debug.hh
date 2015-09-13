<?hh
class Debug {
  public static Vector<string> $Colors = Vector{
    '1;34', // light blue
    '0;32', // green
    '1;33', // yellow
    '1;36', // light cyan
    '1;31', // light red
    '1;35', // light purple
    '0;37', // light grey
    '0;33', // brown
  };
  public static int $ColorsPointer = -1;
  public static Map<string, Debug> $Instances = Map{};
  public static resource $DefaultOutput = STDOUT;

  private resource $Output;
  private bool $Enabled;
  private ?string $Color;
  public function __construct(public string $Prefix, ?resource $Output = null, bool $Enabled = false) {
    if ($Output === null) {
      $Output = static::$DefaultOutput;
    }
    $this->Output = $Output;
    $this->Color = posix_isatty($Output) ? static::$Colors[static::$ColorsPointer = (static::$ColorsPointer + 1 % static::$Colors->count())] : null;
    $this->Enabled = $Enabled || array_key_exists('DEBUG', $_SERVER) ? (bool) preg_match('/^'. str_replace('*', '.*', $_SERVER['DEBUG']) .'$/', $Prefix) : false;
  }
  public function enable() : void {
    $this->Enabled = true;
  }
  public function disable() : void {
    $this->Enabled = false;
  }
  public function log(string $ToWrite) : void {
    if ($this->Enabled) {
      if ($this->Color === null) {
        $ToWrite = date(DATE_RFC850) . ' ' . $ToWrite;
      } else {
        $ToWrite = "  \033[" . $this->Color . 'm' . $this->Prefix . "\033[0m " . $ToWrite;
      }
      $ToWrite .= "\n";
      fwrite($this->Output, $ToWrite);
    } // else { No-Op }
  }

  public static function Log(string $Prefix, string $ToWrite) {
    $Instance = static::$Instances->get($Prefix);
    if ($Instance === null) {
      $Instance = new Debug($Prefix);
      static::$Instances->set($Prefix, $Instance);
    }
    $Instance->log($ToWrite);
  }
}
