with (import <nixpkgs> {});

mkShell {
  buildInputs = [ curl bash python3 python37Packages.flask gnupg  perlPackages.Mojolicious perl ];
}
