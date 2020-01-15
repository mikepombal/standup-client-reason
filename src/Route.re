type t =
  | Login
  | PeopleList
  | GitHubCode
  | NotFound;

let fromUrl =
  fun
  | [] => PeopleList
  | ["login"] => Login
  | ["github"] => GitHubCode
  | ["standup"] => PeopleList
  | _ => NotFound;

let toUrl =
  fun
  | PeopleList => "/standup"
  | Login => "/login"
  | _ => "/404";

let push = route => route |> toUrl |> ReasonReactRouter.push;