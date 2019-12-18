open DesignSystem;

module Classes = {
  let header = () =>
    Css.(
      style([
        display(`flex),
        alignItems(`center),
        justifyContent(`center),
        height(`custom(16) |> Styles.space),
        borderBottom(px(1), `solid, `Primary |> Styles.useColor),
        fontWeight(`num(500)),
        fontSize(`px(28)),
        ...Styles.font(`lg),
      ])
    );
};

[@react.component]
let make = () =>
  <header className={Classes.header()}>
    <div> {ReasonReact.string("Standup")} </div>
  </header>;