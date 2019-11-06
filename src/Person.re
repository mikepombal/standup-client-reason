[@react.component]
let make = (~firstname, ~checked) =>
  <li>
    <input type_="checkbox" checked />
    {ReasonReact.string(firstname)}
  </li>;