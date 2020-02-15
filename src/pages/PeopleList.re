module LastUpdateQueryConfig = [%graphql
  {|
  query LastUpdate {
    getLastUpdate {
      username,
      firstname,
      lastUpdate,
    }
  }
|}
];

module AddUpdatesMutationConfig = [%graphql
  {|
    mutation addUpdates($date: String!, $people: [String!]!) {
        addUpdates(date: $date, usrnames: $people)
    }
|}
];

open ApolloHooks;

// module AddUpdates = ReasonApolloHooks.Mutation.Make(AddUpdatesMutationConfig);

type action =
  | TogglePerson(string)
  | ClearChecks
  | SetDate(string);

type state = {
  date: string,
  listSelected: list(string),
};
let initialState = {date: Helpers.getCurrentDateString(), listSelected: []};

module Classes = {
  open DesignSystem;

  let container = () =>
    Css.(
      style([
        display(`flex),
        maxWidth(`px(800)),
        margin2(~v=px(0), ~h=`auto),
        paddingTop(`px(20)),
      ])
    );
  let list = () =>
    Css.(style([flexGrow(2.0), display(`flex), flexDirection(`column)]));
  let time = () =>
    Css.(
      style([
        flexGrow(0.0),
        display(`flex),
        flexDirection(`column),
        height(`px(150)),
        justifyContent(`spaceAround),
        marginTop(`px(100)),
      ])
    );
  let button = () =>
    Css.(
      style([
        height(`px(50)),
        color(`BodyText |> Styles.useColor),
        backgroundColor(Styles.color(`Primary, Dark)),
        borderColor(Styles.color(`Primary, Dark)),
        borderRadius(`px(5)),
      ])
    );
};

[@react.component]
let make = () => {
  let (_, full) = useQuery(LastUpdateQueryConfig.definition);
  let (addUpdates, _simple, _full) =
    useMutation(
      ~refetchQueries=
        _ => {
          let query = LastUpdateQueryConfig.make();
          [|toQueryObj(query)|];
        },
      AddUpdatesMutationConfig.definition,
    );

  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | TogglePerson(person) =>
          switch (List.find(str => str === person, state.listSelected)) {
          | exception Not_found => {
              ...state,
              listSelected: [person, ...state.listSelected],
            }
          | _ => {
              ...state,
              listSelected:
                List.filter(str => str !== person, state.listSelected),
            }
          }
        | SetDate(date) => {...state, date}
        | ClearChecks => {...state, listSelected: []}
        },
      initialState,
    );
  let onSubmit = () => {
    let variables =
      AddUpdatesMutationConfig.make(
        ~date=state.date,
        ~people=Array.of_list(state.listSelected),
        (),
      )##variables;
    addUpdates(~variables, ())
    |> Js.Promise.then_(
         (_res: Mutation.controlledVariantResult(AddUpdatesMutationConfig.t)) => {
         dispatch(ClearChecks);
         Js.Promise.resolve();
       })
    |> ignore;
  };
  let buttonStyle = Classes.button();
  switch (full) {
  | {loading: true} => <p> {ReasonReact.string("Loading")} </p>
  | {data: Some(d)} =>
    <div className={Classes.container()}>
      <ul className={Classes.list()}>
        {ReasonReact.array(
           Array.map(
             item =>
               <Person
                 key={item##username}
                 firstname={item##firstname}
                 lastUpdate={item##lastUpdate}
                 checked={
                   switch (
                     List.find(
                       str => str === item##username,
                       state.listSelected,
                     )
                   ) {
                   | exception Not_found => false
                   | _ => true
                   }
                 }
                 toggle={_evt => dispatch(TogglePerson(item##username))}
               />,
             d##getLastUpdate,
           ),
         )}
      </ul>
      <div className={Classes.time()}>
        <input
          type_="date"
          value={state.date}
          onChange={evt =>
            dispatch(SetDate(ReactEvent.Form.target(evt)##value))
          }
        />
        <button
          className=buttonStyle
          onClick={_evt => onSubmit()}
          disabled={List.length(state.listSelected) == 0}>
          {ReasonReact.string("Add Updates")}
        </button>
      </div>
    </div>
  | {error: Some(_)} => <p> {ReasonReact.string("Error!")} </p>
  | _ => <p> {ReasonReact.string("Unexpected")} </p>
  };
};