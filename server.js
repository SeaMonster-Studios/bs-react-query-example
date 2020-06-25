const faker = require("faker")
const _ = require("lodash")

module.exports = () => {

  const authedUser = {
    id: faker.random.uuid(),
    name: "Carlton Lassiter",
    avatar: faker.image.avatar()
  };

  const users = _.times(4, () => ({
    id: faker.random.uuid(),
    name: `${faker.name.firstName()} ${faker.name.lastName()},`,
    avatar: faker.image.avatar()
  }));

  let postId = faker.random.uuid();


  const data = {
    users: [authedUser, ...users],
    posts: [
      { id: postId, title: faker.name.title(), body: faker.lorem.paragraphs(), authorId: authedUser.id }
    ],
    comments: _.times(5, () => ({
      id: faker.random.uuid(),
      postId,
      body: faker.lorem.sentences(),
      authorId: users[Math.floor(Math.random() * users.length)].id
    }))
  }
  return data
}