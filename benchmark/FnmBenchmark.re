let run = f => {
  open Benchmark;
  let t0 = Benchmark.make(0L);
  f();
  Benchmark.sub(Benchmark.make(0L), t0).wall *. 1000.;
};

let mktmpdir = () => {
  let base = Filename.get_temp_dir_name();
  let randomPart = Random.int64(100000000000L) |> Int64.to_string;
  Filename.concat(base, "fnm-benchmark-" ++ randomPart);
};

let median = xs => {
  let sorted = List.sort(compare, xs);
  let length = List.length(sorted);
  let index = length / 2 + 1;
  List.nth(sorted, index);
};

let runShellScript = (path, tmpdir) => {
  Lwt_process.exec(
    ~stdout=`Dev_null,
    ~stderr=`Dev_null,
    ("", [|"bash", path, tmpdir|]),
  )
  |> Lwt.map(_ => ())
  |> Lwt_main.run;
};

let log = Printf.eprintf("> %s\n");

let printReport = (~times, scenarios) => {
  Printf.printf(
    "| Tool | Median time taken in ms (%d iterations) |\n",
    times,
  );
  print_endline("| ---- | --------------------------------------- |");
  scenarios
  |> List.map(((name, fn)) => {
       let time = List.init(times, _ => run(fn)) |> median;
       (name, time);
     })
  |> List.iter(((name, timeTaken)) =>
       Printf.sprintf("| %s | %f |", name, timeTaken) |> print_endline
     );
};

Random.self_init();

let fnmdir = mktmpdir();
let nvmdir = mktmpdir();

log("running `before_fnm.sh`");
runShellScript("./benchmark/scripts/before_fnm.sh", fnmdir);

log("running `before_nvm.sh`");
runShellScript("./benchmark/scripts/before_nvm.sh", nvmdir);

log("running `use` benchmarks");

printReport(
  ~times=100,
  [
    ("fnm", () => runShellScript("./benchmark/scripts/fnm.sh", fnmdir)),
    (
      "fnm without `--multi`",
      () =>
        runShellScript("./benchmark/scripts/fnm_not_multishell.sh", fnmdir),
    ),
    ("nvm", () => runShellScript("./benchmark/scripts/nvm.sh", nvmdir)),
  ],
);
