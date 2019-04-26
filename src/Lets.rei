module type Async = {
  type t('a) = Js.Promise.t('a);
  let try_:
    (Js.Promise.t('a), Js.Promise.error => Js.Promise.t('a)) =>
    Js.Promise.t('a);
  let let_: (Js.Promise.t('a), 'a => Js.Promise.t('b)) => Js.Promise.t('b);
  let resolve: 'a => Js.Promise.t('a);
  let reject: exn => Js.Promise.t('a);
  let map: (Js.Promise.t('a), 'a => 'b) => Js.Promise.t('b);
  module Panic: {
    let let_:
      (
        (Js.Promise.error => Js.Promise.t('a), Js.Promise.t('b)),
        'b => Js.Promise.t('c)
      ) =>
      Js.Promise.t('c);
  };
  module Finally: {
    let let_:
      (
        (bool => Js.Promise.t('a), Js.Promise.t('b)),
        'b => Js.Promise.t('c)
      ) =>
      Js.Promise.t('c);
  };
  module Consume: {let let_: (Js.Promise.t('a), 'a => unit) => unit;};
};
module rec Async: Async;
module Result: {
  let let_:
    (Belt.Result.t('a, 'b), 'a => Belt.Result.t('a, 'b)) =>
    Belt.Result.t('a, 'b);
  let try_:
    (Belt.Result.t('a, 'b), 'b => Belt.Result.t('a, 'b)) =>
    Belt.Result.t('a, 'b);
};
module ResultDefault: {
  let let_: ((Belt.Result.t('a, 'b), 'c), 'a => 'c) => 'c;
};
module Guard: {let let_: ((bool, 'a), unit => 'a) => 'a;};
module OptDefault: {
  let let_: ((option('a), 'b), 'a => 'b) => 'b;
  let or_: (option('a), 'a) => 'a;
};
module Opt: {let let_: (option('a), 'a => option('b)) => option('b);};
module OptIf: {let let_: (bool, unit => option('a)) => option('a);};
module OptMap: {let let_: (option('a), 'a => 'b) => option('b);};
module OptOr: {let let_: (option('a), unit => option('a)) => option('a);};
module OptBind: {let let_: (option('a), 'a => unit) => unit;};
