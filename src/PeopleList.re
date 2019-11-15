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

module LastUpdate = ReasonApolloHooks.Query.Make(LastUpdateQueryConfig);
module AddUpdates = ReasonApolloHooks.Mutation.Make(AddUpdatesMutationConfig);

type action =
  | TogglePerson(string)
  | SetDate(string);

type state = {
  date: string,
  listSelected: list(string),
};
let initialState = {date: Helpers.getCurrentDateString(), listSelected: []};

[@react.component]
let make = () => {
  let (_, full) = LastUpdate.use();
  let (addUpdates, _, _) = AddUpdates.use();
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
         (
           res:
             ReasonApolloHooks.Mutation.controlledVariantResult(
               AddUpdatesMutationConfig.t,
             ),
         ) => {
         Js.log(res);
         Js.Promise.resolve();
       })
    |> ignore;
  };
  <div>
    {switch (full) {
     | {loading: true} => <p> {ReasonReact.string("Loading")} </p>
     | {data} =>
       switch (data) {
       | Some(d) =>
         <>
           <ul>
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
           <input
             type_="date"
             value={state.date}
             onChange={evt =>
               dispatch(SetDate(ReactEvent.Form.target(evt)##value))
             }
           />
           <button
             onClick={_evt => onSubmit()}
             disabled={List.length(state.listSelected) == 0}>
             {ReasonReact.string("Add Updates")}
           </button>
           <code>
             {ReasonReact.array(
                Array.of_list(
                  List.map(
                    username =>
                      ReasonReact.string("\"" ++ username ++ "\", "),
                    state.listSelected,
                  ),
                ),
              )}
           </code>
         </>
       | None => <p> {ReasonReact.string("No Data")} </p>
       }
     }}
  </div>;
};