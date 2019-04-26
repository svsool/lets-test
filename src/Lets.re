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

module rec Async: Async = {
  type t('a) = Js.Promise.t('a);
  let try_ = (promise, continuation) =>
    Js.Promise.catch(continuation, promise);
  let let_ = (promise, continuation) =>
    Js.Promise.then_(continuation, promise);
  let resolve = Js.Promise.resolve;
  let reject = Js.Promise.reject;
  let map = (promise, fn) =>
    Js.Promise.then_(v => Js.Promise.resolve(fn(v)), promise);

  module Panic = {
    let let_ = ((rollback, promise), continuation) =>
      promise
      |> Js.Promise.catch(e => {
           let%Async _ = rollback(e);
           Js.Promise.reject(Obj.magic(e));
         })
      |> Js.Promise.then_(continuation);
  };

  module Finally = {
    let let_ = ((finally, promise), continuation) =>
      promise
      |> Js.Promise.catch(e => {
           let%Async _ = finally(true);
           Js.Promise.reject(Obj.magic(e));
         })
      |> Js.Promise.then_(v => {
           let%Async _ = finally(false);
           Js.Promise.resolve(v);
         })
      |> Js.Promise.then_(continuation);
  };

  module Consume = {
    let let_ = (promise, cont) =>
      Js.Promise.then_(
        value => {
          let () = cont(value);
          Js.Promise.resolve();
        },
        promise,
      )
      |> ignore;
  };
};

module Result = {
  let let_ = (v, fn) =>
    switch (v) {
    | Belt.Result.Ok(o) => fn(o)
    | Error(_) => v
    };
  let try_ = (v, fn) =>
    switch (v) {
    | Belt.Result.Ok(_) => v
    | Error(e) => fn(e)
    };
};

module ResultDefault = {
  let let_ = ((a, default), fn) =>
    switch (a) {
    | Belt.Result.Ok(o) => fn(o)
    | Error(_) => default
    };
};

module Guard = {
  let let_ = ((condition, default), continuation) =>
    if (condition) {
      continuation();
    } else {
      default;
    };
};

module OptDefault = {
  let let_ = ((a, default), b) =>
    switch (a) {
    | None => default
    | Some(x) => b(x)
    };
  let or_ = (v, default) =>
    switch (v) {
    | None => default
    | Some(c) => c
    };
};

module Opt = {
  let let_ = (a, b) =>
    switch (a) {
    | None => None
    | Some(x) => b(x)
    };
};

module OptIf = {
  let let_ = (a, b) =>
    if (a) {
      b();
    } else {
      None;
    };
};

module OptMap = {
  let let_ = (a, b) =>
    switch (a) {
    | None => None
    | Some(x) => Some(b(x))
    };
};

module OptOr = {
  let let_ = (a, b) =>
    switch (a) {
    | None => b()
    | Some(_) => a
    };
};

module OptBind = {
  let let_ = (a, b) =>
    switch (a) {
    | None => ()
    | Some(x) => b(x)
    };
};
