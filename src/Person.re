[@react.component]
let make = (~username, ~firstname) =>
  <li>
    {ReasonReact.string(username)}
    {ReasonReact.string(" - ")}
    {ReasonReact.string(firstname)}
  </li>;