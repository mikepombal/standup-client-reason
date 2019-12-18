module Classes = {
  open DesignSystem;
  let label = checked => {
    let bgColor = (checked ? `Secondary : `CardBg) |> Styles.useColor;
    Css.(
      style([
        display(`flex),
        alignItems(`center),
        width(`auto),
        backgroundColor(bgColor),
        borderRadius(`px(3)),
        margin2(~v=px(5), ~h=px(40)),
        height(`px(70)),
        cursor(`pointer),
        padding2(~v=px(0), ~h=px(20)),
      ])
    );
  };

  let input = () => Css.(style([opacity(0.0)]));

  let labels = () =>
    Css.(
      style([
        display(`flex),
        justifyContent(`spaceBetween),
        width(`percent(100.0)),
      ])
    );
};

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
    <label className={Classes.label(checked)}>
      <input
        type_="checkbox"
        checked
        onChange=toggle
        className={Classes.input()}
      />
      <div className={Classes.labels()}>
        <span> {ReasonReact.string(firstname)} </span>
        <span> {ReasonReact.string(" (" ++ days ++ ")")} </span>
      </div>
    </label>
  </li>;
};