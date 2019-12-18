module LoginQuery = [%graphql
  {|
  mutation login($username: String!, $password: String!) {
    login(
        usrname: $username
        password: $password
    ) {
      token
      user {
        username
        firstname
        surname
      }
    }
  }
|}
];

module LoginMutation = ReasonApolloHooks.Mutation.Make(LoginQuery);

type state = {
  username: string,
  password: string,
};
type action =
  | UpdateUsername(string)
  | UpdatePassword(string);

[@react.component]
let make = (~setLogin) => {
  let (login, _, _) = LoginMutation.use();
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | UpdateUsername(str) => {...state, username: str}
        | UpdatePassword(str) => {...state, password: str}
        },
      {username: "", password: ""},
    );

  let onSubmit = () => {
    let variables =
      LoginQuery.make(~username=state.username, ~password=state.password, ())##variables;
    login(~variables, ())
    |> Js.Promise.then_(
         (
           res:
             ReasonApolloHooks.Mutation.controlledVariantResult(LoginQuery.t),
         ) => {
         switch (res) {
         | NoData
         | Loading
         | Called
         | Error(_) => ()
         | Data(data) =>
           Js.log("Some data!");
           switch (data##login) {
           | Some(login) =>
             Storage.saveTokenToStorage(login##token);
             Storage.saveUserToStorage(login##user);
             setLogin(_ => true);
           | None => ()
           };
         };

         Js.Promise.resolve();
       })
    |> ignore;
  };

  <p>
    <input
      placeholder="Username"
      value={state.username}
      onChange={evt =>
        dispatch(UpdateUsername(ReactEvent.Form.target(evt)##value))
      }
    />
    <input
      type_="password"
      placeholder="Password"
      value={state.password}
      onChange={evt =>
        dispatch(UpdatePassword(ReactEvent.Form.target(evt)##value))
      }
    />
    <button onClick={_evt => onSubmit()}>
      {ReasonReact.string("Login")}
    </button>
  </p>;
};