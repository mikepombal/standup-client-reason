[@bs.module "../assets/GitHub-Mark-Light-64px.png"]
external src: string = "default";

open ApolloHooks;

// module LoginQuery = [%graphql
//   {|
//   mutation login($username: String!, $password: String!) {
//     login(
//         usrname: $username
//         password: $password
//     ) {
//       token
//       user {
//         username
//         firstname
//         surname
//       }
//     }
//   }
// |}
// ];

module GithubLoginUrlQuery = [%graphql
  {|
  query GithubLoginUrl {
    githubLoginUrl
  }
|}
];

// module LoginMutation = ReasonApolloHooks.Mutation.Make(LoginQuery);
// module GithubLoginUrl = ReasonApolloHooks.Query.Make(GithubLoginUrlQuery);

type state = {
  username: string,
  password: string,
};
type action =
  | UpdateUsername(string)
  | UpdatePassword(string);

module Classes = {
  open DesignSystem;

  let mainContainer = () =>
    Css.(
      style([
        display(`flex),
        width(`percent(100.0)),
        height(`vh(80.0)),
        justifyContent(`center),
        alignItems(`center),
      ])
    );

  let loginContainer = () =>
    Css.(
      style([display(`flex), flexDirection(`column), alignItems(`center)])
    );

  let button = () =>
    Css.(
      style([
        height(`px(60)),
        color(`BodyText |> Styles.useColor),
        backgroundColor(Styles.color(`Primary, Dark)),
        borderColor(Styles.color(`Primary, Dark)),
        borderRadius(`px(5)),
        marginTop(`px(40)),
        padding2(~v=px(0), ~h=px(30)),
      ])
    );

  let githubImage = () => Css.(style([marginLeft(`px(20))]));
};

[@react.component]
let make = () => {
  // let (login, _, _) = useMutation(LoginMutation.use);

  let (simple, _full) = useQuery(GithubLoginUrlQuery.definition); //GithubLoginUrl.use();
  // let (state, dispatch) =
  //   React.useReducer(
  //     (state, action) =>
  //       switch (action) {
  //       | UpdateUsername(str) => {...state, username: str}
  //       | UpdatePassword(str) => {...state, password: str}
  //       },
  //     {username: "", password: ""},
  //   );

  // let onSubmit = () => {
  //   let variables =
  //     LoginQuery.make(~username=state.username, ~password=state.password, ())##variables;
  //   login(~variables, ())
  //   |> Js.Promise.then_(
  //        (
  //          res:
  //            ReasonApolloHooks.Mutation.controlledVariantResult(LoginQuery.t),
  //        ) => {
  //        switch (res) {
  //        | NoData
  //        | Loading
  //        | Called
  //        | Error(_) => ()
  //        | Data(data) =>
  //          Js.log("Some data!");
  //          switch (data##login) {
  //          | Some(login) =>
  //            Storage.saveTokenToStorage(login##token);
  //            Storage.saveUserToStorage(login##user);
  //            setLogin(_ => true);
  //          | None => ()
  //          };
  //        };

  //        Js.Promise.resolve();
  //      })
  //   |> ignore;
  // };

  // <p>
  //   <input
  //     placeholder="Username"
  //     value={state.username}
  //     onChange={evt =>
  //       dispatch(UpdateUsername(ReactEvent.Form.target(evt)##value))
  //     }
  //   />
  //   <input
  //     type_="password"
  //     placeholder="Password"
  //     value={state.password}
  //     onChange={evt =>
  //       dispatch(UpdatePassword(ReactEvent.Form.target(evt)##value))
  //     }
  //   />
  //   <button onClick={_evt => onSubmit()}>
  //     {ReasonReact.string("Login")}
  //   </button>
  // </p>;
  <div className={Classes.mainContainer()}>
    {switch (simple) {
     | Data(data) =>
       <div className={Classes.loginContainer()} action={data##githubLoginUrl}>
         {ReasonReact.string("Please login using your GitHub account")}
         <a href={data##githubLoginUrl}>
           <button className={Classes.button()}>
             {ReasonReact.string("Sign In With GitHub")}
             <img
               className={Classes.githubImage()}
               src
               height="40"
               width="40"
             />
           </button>
         </a>
       </div>
     | _ => ReasonReact.string("Loading...")
     }}
  </div>;
};