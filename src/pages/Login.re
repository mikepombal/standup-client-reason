[@bs.module "../assets/GitHub-Mark-Light-64px.png"]
external src: string = "default";

open ApolloHooks;

module GithubLoginUrlQuery = [%graphql
  {|
  query GithubLoginUrl {
    githubLoginUrl
  }
|}
];

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
  let (simple, _full) = useQuery(GithubLoginUrlQuery.definition);
  let buttonStyle = Classes.button();

  <div className={Classes.mainContainer()}>
    {switch (simple) {
     | Data(data) =>
       <div className={Classes.loginContainer()} action={data##githubLoginUrl}>
         {ReasonReact.string("Please login using your GitHub account")}
         <a href={data##githubLoginUrl}>
           <button className=buttonStyle>
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