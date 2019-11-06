let add0PrefixWhenNeeded = str =>
  switch (String.length(str)) {
  | 1 => "0" ++ str
  | _ => str
  };

let getDateString = date =>
  Js.Float.toString(Js.Date.getFullYear(date))
  ++ "-"
  ++ add0PrefixWhenNeeded(Js.Float.toString(Js.Date.getMonth(date) +. 1.0))
  ++ "-"
  ++ add0PrefixWhenNeeded(Js.Float.toString(Js.Date.getDate(date)));

let getCurrentDateString = () => {
  let now = Js.Date.make();
  getDateString(now);
};