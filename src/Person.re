[@react.component]
let make = (~firstname, ~checked, ~toggle) =>
  <li>
    <input type_="checkbox" checked onChange=toggle />
    {ReasonReact.string(firstname)}
  </li>;