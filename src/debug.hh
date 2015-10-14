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
  public static ?resource $DefaultOutput = null;

  private resource $Output;
  private bool $Enabled;
  private ?string $Color;
  public function __construct(public string $Prefix, ?resource $Output = null, bool $Enabled = false) {
    if ($Output === null) {
      $Output = static::$DefaultOutput;
      if ($Output === null) {
        if (defined('STDOUT')) {
          $Output = STDOUT;
        } else throw new Exception('No output stream defined');
      }
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
  public function log(...$Contents) : void {
    if ($this->Enabled) {
      if ($this->Color === null) {
        $Prefix = date(DATE_RFC850) . ' ➸ ' . $this->Prefix . "\n ";
      } else {
        $Prefix = "  \033[" . $this->Color . 'm' . $this->Prefix . "\033[0m ";
      }
      fwrite($this->Output, $Prefix . static::format(...$Contents). "\n");
    } // else { No-Op }
  }

  public static function this(string $Prefix, ...$Contents) {
    $Instance = static::$Instances->get($Prefix);
    if ($Instance === null) {
      $Instance = new Debug($Prefix);
      static::$Instances->set($Prefix, $Instance);
    }
    $Instance->log(...$Contents);
  }
  public static function format(...$Contents):string {
    $ToWrite = [];
    foreach ($Contents as $Entry) {
      if (is_string($Entry) || is_int($Entry)) {
        $ToWrite[] = $Entry;
      } else if (is_bool($Entry)) {
        $ToWrite[] = $Entry ? 'true' : 'false';
      } else if (is_null($Entry)) {
        $ToWrite[] = 'null';
      } else if (is_object($Entry)) {
        $WriteEntry = get_class($Entry);
        if ($Entry instanceof Exception) {
          $WriteEntry .= ' { '. $Entry->getMessage() ." }\n    ".implode("\n    ", explode("\n", $Entry->getTraceAsString()));
        } else if ($Entry instanceof JsonSerializable) {
          $WriteEntry .= ' { '. substr(json_encode($Entry), 1, -1) .' }';
        } else if ($Entry instanceof Stringish) {
          $WriteEntry .= ' { '. $Entry .' }';
        } else {
          $WriteEntry .= ' { Object }';
        }
        $ToWrite[] = $WriteEntry;
      } else if (is_array($Entry)) {
        $ToWrite[] = '[ ' . static::format(...$Entry) . ' ]';
      } // else { No-Op }
    }
    return implode(' ', $ToWrite);
  }
}
