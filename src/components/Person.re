[@react.component]
let make = (~firstname, ~checked, ~lastUpdate, ~toggle) => {
  let days =
    switch (lastUpdate) {
    | None => "New"
    | Some(date) =>
      let diff =
        Js_date.getTime(Js.Date.make())
        -. Js_date.getTime(Js.Date.fromString(date));
      let numDays = Js.Math.floor(diff /. (24. *. 3660. *. 1000.));
      switch (numDays) {
      | 0
      | 1 => string_of_int(numDays) ++ " day"
      | _ => string_of_int(numDays) ++ " days"
      };
    };
  <li>
    <input type_="checkbox" checked onChange=toggle />
    {ReasonReact.string(firstname ++ " (" ++ days ++ ")")}
  </li>;
};