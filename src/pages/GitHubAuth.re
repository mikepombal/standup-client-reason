open ApolloHooks;

module AuthorizeWithGithubMutation = [%graphql
  {|
  mutation authorizeWithGithub($code: String!) {
    authorizeWithGithub(
        code: $code
    ) {
      token
      user {
        username
        firstname
        surname
        role
      }
    }
  }
|}
];

let getCode = querystring => {
  let arr = Js.String.split("=", querystring);
  if (Array.length(arr) > 1) {
    Some(arr[1]);
  } else {
    None;
  };
};

[@react.component]
let make = (~querystring) => {
  let (authorize, _simple, _full) =
    useMutation(AuthorizeWithGithubMutation.definition);
  React.useEffect1(
    () => {
      let code = getCode(querystring);
      switch (code) {
      | Some(code) =>
        Js.log("There is a surprise: " ++ code);
        let variables =
          AuthorizeWithGithubMutation.make(~code, ())##variables;

        authorize(~variables, ())
        |> Js.Promise.then_(
             (
               res:
                 Mutation.controlledVariantResult(
                   AuthorizeWithGithubMutation.t,
                 ),
             ) => {
             switch (res) {
             | NoData
             | Loading
             | Called
             | Error(_) => ()
             | Data(data) =>
               Storage.saveUserToStorage(data##authorizeWithGithub);
               Route.push(PeopleList);
             };
             Js.Promise.resolve();
           })
        |> Js.Promise.catch(_error => {
             Js.log("Something wrong");
             Js.Promise.resolve();
           })
        |> ignore;
        ();
      | None => Js.log("Oops not good")
      };
      None;
    },
    [|querystring|],
  );

  let arr = Js.String.split("=", querystring);
  if (Array.length(arr) > 1) {
    <div> {React.string("Please wait while getting the token...")} </div>;
  } else {
    <div> {React.string("Oops, something wrong happened!")} </div>;
  };
};