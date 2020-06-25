open Belt;

module Style = {
  open Css;
};

[@decco]
type post = {
  id: string,
  title: string,
  body: string,
  authorId: string,
};

[@decco]
type posts = array(post);

module Queries = {
  module Posts = {
    include ReactQuery.Query({
      type key = string;
      type data = Result.t(posts, Decco.decodeError);
      type vars = unit;
    });

    let endpoint = {j|/api/posts|j}->Some;

    let use = () => {
      use(~key=endpoint, (key, _) =>
        Js.Promise.(
          Fetch.fetch(key)
          |> then_(Fetch.Response.json)
          |> then_(json => json->posts_decode->resolve)
        )
      );
    };
  };
};

[@react.component]
let make = () => {
  let usersQ = Users.Queries.use();
  let postsQ = Queries.Posts.use();

  switch (usersQ.status, postsQ.status) {
  | (Success(Ok(users)), Success(Ok(posts))) =>
    <div>
      {posts
       ->Array.map(post =>
           <React.Fragment key={post.id}>
             {switch (
                users->Array.getBy(u => u.id == post.authorId),
                users->Array.get(0),
              ) {
              | (_, None)
              | (None, _) => <ErrorMessage />
              | (Some(author), Some(authedUser)) =>
                <Card
                  author
                  footer={<AddCommentForm postId={post.id} authedUser />}>
                  post.body->React.string
                </Card>
              }}
           </React.Fragment>
         )
       ->React.array}
    </div>
  | (Loading, _)
  | (_, Loading) => <Loader />
  | (Success(Error(err)), _)
  | (_, Success(Error(err))) =>
    Js.log(err);
    <ErrorMessage />;
  | (Error(err), _)
  | (_, Error(err)) =>
    Js.log(err);
    <ErrorMessage />;
  };
};